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

let roleToString = role =>
  switch (role) {
  | `ADMIN => "admin"
  | `USER => "user"
  | `UNAUTHORIZED => "unauthorized"
  | `EDITOR => "editor"
  };

let stringToRole = role =>
  switch (role) {
  | "admin" => `ADMIN
  | "user" => `USER
  | "unauthorized" => `UNAUTHORIZED
  | "editor" => `EDITOR
  | role => Js.Exn.raiseError({j|Unrecognized role $role|j})
  };

module UserQuery = ReasonApollo.CreateQuery(User);

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
    | Role => state.role |> roleToString
    };
  let set = ((key, value), state) =>
    switch (key) {
    | Email => {...state, email: value}
    | Role => {...state, role: value |> stringToRole}
    };
};

type interface = Form.interface(FormConfig.key, FormConfig.value);

module UserForm = Form.Make(FormConfig);

let onSubmit = values => Js.log(values);

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
            onChange=(
              event => {
                let obj =
                  event
                  |> ReactEventRe.Form.target
                  |> ReactDOMRe.domElementToObj;
                obj##value |> onChange(Email);
              }
            )
          />
          <select
            value=(getValue(Role))
            onChange=(
              event => {
                let obj =
                  event
                  |> ReactEventRe.Form.target
                  |> ReactDOMRe.domElementToObj;
                obj##value |> onChange(Role);
              }
            )>
            <option value="admin"> ("Admin" |> ReasonReact.string) </option>
            <option value="editor"> ("Editor" |> ReasonReact.string) </option>
            <option value="user"> ("User" |> ReasonReact.string) </option>
            <option value="unauthorized">
              ("Unauthorized" |> ReasonReact.string)
            </option>
          </select>
          <Button type_=Submit> ...("Save" |> ReasonReact.string) </Button>
        </form>;

      <div>
        <UserQuery variables=userQuery##variables>
          ...(
               ({result}) =>
                 switch (result) {
                 | Loading => <Loader />
                 | Error(error) => <ApolloError error />
                 | Data(response) =>
                   <div key=response##user##id>
                     <UserForm
                       initialValues={
                         email: response##user##email,
                         role: response##user##role,
                       }
                       onSubmit>
                       ...renderUserForm
                     </UserForm>
                   </div>
                 }
             )
        </UserQuery>
      </div>;
    },
  };
};