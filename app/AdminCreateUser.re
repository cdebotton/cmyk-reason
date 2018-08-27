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
  let firstname = style([`declaration(("gridColumn", "span 2"))]);
  let lastname = style([`declaration(("gridColumn", "span 2"))]);
  let role = style([`declaration(("gridColumn", "span 4"))]);

  let save = style([`declaration(("gridColumn", "3 / span 1"))]);
  let cancel = style([`declaration(("gridColumn", "span 1"))]);
};

module FormConfig = {
  type t = {
    email: string,
    password: string,
    firstname: string,
    lastname: string,
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
    | Firstname => state.firstname
    | Lastname => state.lastname
    | Role => state.role |> Role.roleToString
    };

  let set = ((key, value), state) =>
    switch (key) {
    | Email => {...state, email: value}
    | Password => {...state, password: value}
    | Firstname => {...state, firstname: value}
    | Lastname => {...state, lastname: value}
    | Role => {...state, role: value |> Role.stringToRole}
    };
};

module CreateUserForm = Form.Make(FormConfig);

let component = ReasonReact.statelessComponent("AdminCreateUser");

let make = (~onSave, ~onCancel, _children) => {
  let handleCancel = event => {
    event->ReactEvent.Mouse.preventDefault;
    onCancel();
  };

  {
    ...component,
    render: _self =>
      <CreateUserForm
        initialValues={
          email: "",
          password: "",
          firstname: "",
          lastname: "",
          role: `UNAUTHORIZED,
        }
        onSubmit={_ => onSave()}>
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
                       {label: "Admin", value: `ADMIN |> Role.roleToString},
                       {label: "Editor", value: `EDITOR |> Role.roleToString},
                       {label: "User", value: `USER |> Role.roleToString},
                       {
                         label: "Unauthorized",
                         value: `UNAUTHORIZED |> Role.roleToString,
                       },
                     ]
                   />
                   <Input
                     className=Styles.firstname
                     placeholder="First name"
                     value={getValue(Firstname)}
                     onChange={onChange(Firstname)}
                   />
                   <Input
                     className=Styles.lastname
                     placeholder="Last name"
                     value={getValue(Lastname)}
                     onChange={onChange(Lastname)}
                   />
                   <Button className=Styles.save type_=Submit>
                     {"Create" |> ReasonReact.string}
                   </Button>
                   <Button
                     className=Styles.cancel type_=Reset onClick=handleCancel>
                     {"Cancel" |> ReasonReact.string}
                   </Button>
                 </form>
               </div>
           }
      </CreateUserForm>,
  };
};