module Styles = {
  let form =
    Css.(
      style([
        display(grid),
        gridTemplateColumns([1. |. fr, 1. |. fr, 1. |. fr]),
        gridGap(1. |. rem),
      ])
    );

  let heading = Css.(style([gridColumn(1, 4)]));

  let email = Css.(style([gridColumn(1, 3)]));
};

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

module UpdateUser = [%graphql
  {|
  mutation UpdateUser($data: UserUpdateInput!, $where: UserWhereUniqueInput!) {
    updateUser(data: $data, where: $where) {
      id
      email
      role
    }
  }
|}
];

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

let onSubmit =
    (userId, mutate: UpdateUserMutation.apolloMutation, values: FormConfig.t) => {
  let updateUser =
    UpdateUser.make(
      ~data={"email": values.email, "role": values.role},
      ~where={"id": Some(userId), "email": None},
      (),
    );

  mutate(~variables=updateUser##variables, ()) |> ignore;
};

let component = ReasonReact.statelessComponent("AdminUpdateUser");

let make = (~userId, _children) => {
  let userQuery = User.make(~where={"id": Some(userId), "email": None}, ());
  {
    ...component,
    render: _self => {
      let renderUserForm = ({onChange, getValue, handleSubmit}: interface) =>
        <form className=Styles.form onSubmit=handleSubmit>
          <Heading className=Styles.heading level=3>
            ("Edit " ++ getValue(Email) |> ReasonReact.string)
          </Heading>
          <Input
            className=Styles.email
            placeholder="Email"
            value=(getValue(FormConfig.Email))
            onChange=(onChange(Email))
          />
          <Select
            placeholder="Role"
            value=(getValue(FormConfig.Role))
            onChange=(onChange(Role))
            options=[
              {label: "Admin", value: `ADMIN |> Role.roleToString},
              {label: "Editor", value: `EDITOR |> Role.roleToString},
              {label: "User", value: `USER |> Role.roleToString},
              {
                label: "Unauthorized",
                value: `UNAUTHORIZED |> Role.roleToString,
              },
            ]
          />
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
                     ...(
                          (mutate, {result}) =>
                            switch (result) {
                            | Data(_response) =>
                              <Redirect to_="/admin/users" />
                            | Error(_)
                            | Loading
                            | NotCalled =>
                              <UserForm
                                initialValues={
                                  email: response##user##email,
                                  role: response##user##role,
                                }
                                onSubmit=(
                                  onSubmit(response##user##id, mutate)
                                )>
                                ...renderUserForm
                              </UserForm>
                            }
                        )
                   </UpdateUserMutation>
                 </div>
               }
           )
      </UserQuery>;
    },
  };
};