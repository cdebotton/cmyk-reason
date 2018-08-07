module User = [%graphql
  {|
     query User($where: UserWhereUniqueInput!) {
       user(where: $where) {
         id
         email
         role
       }
     }
     |}
];

module UserQuery = ReasonApollo.CreateQuery(User);

module UpdateUser = [%graphql {|
  mutation UpdateUser($data: UserUpdateInput!, $where: UserWhereUniqueInput!) {
    updateUser(data: $data, where: $where) {
      id
      email
      role
    }
  }
|}];

module UpdateUserMutation = ReasonApollo.CreateMutation(UpdateUser);

module FormConfig = {
  type value = string;
  type t = {
    email: string,
    role: [ | `ADMIN | `EDITOR | `UNAUTHORIZED | `USER],
  };
  type key =
    | Email
    | Role;
  let debugName = "UserForm";
  let get = (key, state) =>
    switch (key) {
    | Email => state.email
    | Role => state.role |> Role.roleToString
    };
  let set = ((key, value), state) =>
    switch (key) {
    | Email => {...state, email: value}
    | Role => {...state, role: value |> Role.stringToRole}
    };
};

type interface = Form.interface(FormConfig.key, FormConfig.value);

module UserForm = Form.Make(FormConfig);

let onSubmit = (userId, mutate: UpdateUserMutation.apolloMutation, values: FormConfig.t) => {
  let updateUser = UpdateUser.make(
    ~data={
      "email": values.email,
      "role": values.role,
    },
    ~where={
      "id": Some(userId),
      "email": None
    },
    ()
  );

  mutate(~variables=updateUser##variables, ()) |> ignore;
};

let component = ReasonReact.statelessComponent("AdminUser");

let make = (~userId, _children) => {
  let userQuery = User.make(~where={"id": Some(userId), "email": None}, ());
  {
    ...component,
    render: _self => {
      let renderUserForm = ({onChange, getValue, handleSubmit}: interface) =>
        <form onSubmit=handleSubmit>
          <Heading level=3>
            ("Edit " ++ getValue(Email) |> ReasonReact.string)
          </Heading>
          <Input
            placeholder="Email"
            value=(getValue(FormConfig.Email))
            onChange=(onChange(Email))
          />
          <select
            value=(getValue(Role))
            onChange=(
              event => ReactEvent.Form.(event->target##value) |> onChange(Role) 
            )>
            <option value="admin"> ("Admin" |> ReasonReact.string) </option>
            <option value="editor"> ("Editor" |> ReasonReact.string) </option>
            <option value="user"> ("User" |> ReasonReact.string) </option>
            <option value="unauthorized">
              ("Unauthorized" |> ReasonReact.string)
            </option>
          </select>
          <Button type_=Submit> ("Save" |> ReasonReact.string) </Button>
        </form>;


      <UserQuery variables=userQuery##variables>
        ...(
              ({result}) =>
                switch (result) {
                | Loading => <Loader />
                | Error(error) => <ApolloError error />
                | Data(response) =>
                  <div key=response##user##id>
                    <UpdateUserMutation>
                      ...((mutate, _) => 
                        <UserForm
                          initialValues={
                            email: response##user##email,
                            role: response##user##role,
                          }
                          onSubmit=onSubmit(response##user##id, mutate)>
                          ...renderUserForm
                        </UserForm>
                      )
                    </UpdateUserMutation>
                  </div>
                }
            )
      </UserQuery>;
    },
  };
};