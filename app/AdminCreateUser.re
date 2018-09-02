module Styles = {
  open Css;

  let container = style([]);

  let form =
    style([
      backgroundColor(white),
      padding(rem(2.)),
      display(grid),
      gridGap(rem(1.)),
      gridTemplateColumns([fr(1.), fr(1.), fr(1.), fr(1.)]),
    ]);

  let email = style([`declaration(("gridColumn", "span 2"))]);
  let password = style([`declaration(("gridColumn", "span 2"))]);
  let firstName = style([`declaration(("gridColumn", "span 2"))]);
  let lastName = style([`declaration(("gridColumn", "span 2"))]);
  let role = style([`declaration(("gridColumn", "span 4"))]);

  let cancel = style([`declaration(("gridColumn", "3 / span 1"))]);
  let save = style([`declaration(("gridColumn", "span 1"))]);
};

module FormConfig = {
  type t = {
    email: string,
    password: string,
    firstName: string,
    lastName: string,
    role: [ | `ADMIN | `EDITOR | `UNAUTHORIZED | `USER],
  };

  type value = string;

  type key =
    | Email
    | Password
    | Firstname
    | Lastname
    | Role;

  let debugName = "AdminCreateUserForm";

  let get = (key, state) =>
    switch (key) {
    | Email => state.email
    | Password => state.password
    | Firstname => state.firstName
    | Lastname => state.lastName
    | Role => state.role |> Role.roleToString
    };

  let set = ((key, value), state) =>
    switch (key) {
    | Email => {...state, email: value}
    | Password => {...state, password: value}
    | Firstname => {...state, firstName: value}
    | Lastname => {...state, lastName: value}
    | Role => {...state, role: value |> Role.stringToRole}
    };
};

module UserCreate = [%graphql
  {|
  mutation CreateUser($data: UserCreateInput!) {
    createUser(data: $data) {
      id
      email
      role
      profile {
        firstName
        lastName
      }
    }
  }
  |}
];

module UserCreateMutation = ReasonApollo.CreateMutation(UserCreate);

module CreateUserForm = Form.Make(FormConfig);

let component = ReasonReact.statelessComponent("AdminCreateUser");

let make = (~onSave, ~onCancel, _children) => {
  let handleCancel = event => {
    event->ReactEvent.Mouse.preventDefault;
    onCancel();
  };

  let onSubmit =
      (mutate: UserCreateMutation.apolloMutation, values: FormConfig.t) => {
    let userCreate =
      UserCreate.make(
        ~data={
          "email": values.email,
          "password": values.password,
          "firstName": values.firstName,
          "lastName": values.lastName,
          "role": values.role,
        },
        (),
      );
    mutate(~variables=userCreate##variables, ()) |> ignore;
    onSave();
  };

  {
    ...component,
    render: _self =>
      <UserCreateMutation>
        ...{
             (mutate, _) =>
               <CreateUserForm
                 initialValues={
                   email: "",
                   password: "",
                   firstName: "",
                   lastName: "",
                   role: `UNAUTHORIZED,
                 }
                 onSubmit={onSubmit(mutate)}>
                 ...{
                      ({getValue, onChange, handleSubmit}) =>
                        <div className=Styles.container>
                          <form className=Styles.form onSubmit=handleSubmit>
                            <Input
                              className=Styles.email
                              placeholder="Email"
                              value={getValue(Email)}
                              onChange={onChange(Email)}
                            />
                            <Input
                              className=Styles.password
                              type_=Password
                              placeholder="Password"
                              value={getValue(Password)}
                              onChange={onChange(Password)}
                            />
                            <Select
                              className=Styles.role
                              placeholder="Role"
                              value={getValue(FormConfig.Role)}
                              onChange={onChange(Role)}
                              options=[
                                {
                                  label: "Admin",
                                  value: `ADMIN |> Role.roleToString,
                                },
                                {
                                  label: "Editor",
                                  value: `EDITOR |> Role.roleToString,
                                },
                                {
                                  label: "User",
                                  value: `USER |> Role.roleToString,
                                },
                                {
                                  label: "Unauthorized",
                                  value: `UNAUTHORIZED |> Role.roleToString,
                                },
                              ]
                            />
                            <Input
                              className=Styles.firstName
                              placeholder="First name"
                              value={getValue(Firstname)}
                              onChange={onChange(Firstname)}
                            />
                            <Input
                              className=Styles.lastName
                              placeholder="Last name"
                              value={getValue(Lastname)}
                              onChange={onChange(Lastname)}
                            />
                            <Button
                              className=Styles.cancel
                              type_=Reset
                              onClick=handleCancel>
                              {"Cancel" |> ReasonReact.string}
                            </Button>
                            <Button className=Styles.save type_=Submit>
                              {"Create" |> ReasonReact.string}
                            </Button>
                          </form>
                        </div>
                    }
               </CreateUserForm>
           }
      </UserCreateMutation>,
  };
};