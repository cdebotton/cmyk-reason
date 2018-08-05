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

module UserForm = Form.Make(FormConfig);

let onSubmit = values => Js.log(values);

let component = ReasonReact.statelessComponent("AdminUser");

let make = (~userId, _children) => {
  let userQuery = User.make(~where={"id": Some(userId), "email": None}, ());
  {
    ...component,
    render: _self =>
      <div>
        <UserQuery variables=userQuery##variables>
          ...(
               ({result}) =>
                 switch (result) {
                 | Loading => <Loader />
                 | Error(error) => <ApolloError error />
                 | Data(response) =>
                   <div>
                     <p> (response##user##id |> ReasonReact.string) </p>
                     <UserForm
                       initialValues={
                         email: response##user##email,
                         role: response##user##role,
                       }
                       onSubmit>
                       ...(
                            ({onChange, getValue}) =>
                              <form>
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
                                  onChange=(
                                    event => {
                                      let obj =
                                        event
                                        |> ReactEventRe.Form.target
                                        |> ReactDOMRe.domElementToObj;
                                      obj##value |> onChange(Role);
                                    }
                                  )>
                                  <option
                                    value="admin"
                                    selected=(getValue(Role) === "admin")>
                                    ("Admin" |> ReasonReact.string)
                                  </option>
                                  <option
                                    value="editor"
                                    selected=(getValue(Role) === "editor")>
                                    ("Editor" |> ReasonReact.string)
                                  </option>
                                  <option
                                    value="user"
                                    selected=(getValue(Role) === "user")>
                                    ("User" |> ReasonReact.string)
                                  </option>
                                  <option
                                    value="unauthorized"
                                    selected=(
                                      getValue(Role) === "unauthorized"
                                    )>
                                    ("Unauthorized" |> ReasonReact.string)
                                  </option>
                                </select>
                              </form>
                          )
                     </UserForm>
                   </div>
                 }
             )
        </UserQuery>
      </div>,
  };
};
1;