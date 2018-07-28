module LoginConfig = {
  type t = {
    email: string,
    password: string,
  };
  type key =
    | Email
    | Password;
  type value = string;

  let debugName = "LoginForm";
  let get = (key, state) =>
    switch (key) {
    | Email => state.email
    | Password => state.password
    };
  let set = ((key, value), state) =>
    switch (key) {
    | Email => {...state, email: value}
    | Password => {...state, password: value}
    };
};

module LoginForm = Form.Make(LoginConfig);

module Styles = {
  open Css;

  let page =
    style([width(100. |. vw), minHeight(100. |. vh), overflow(auto)]);
};

let component = ReasonReact.statelessComponent("Login");

let make = _children => {
  ...component,
  render: _self =>
    <div className=Styles.page>
      <LoginForm initialValues={email: "", password: ""}>
        ...(
             ({getValue, onChange}) =>
               <form>
                 <Heading level=2> ("Login" |> ReasonReact.string) </Heading>
                 <Input
                   placeholder="Email"
                   value=(getValue(Email))
                   onChange=(
                     event =>
                       event
                       |> Form.valueFromEvent
                       |> (value => value |> onChange(Email))
                   )
                 />
                 <Input
                   placeholder="Password"
                   type_=Password
                   value=(getValue(Password))
                   name="password"
                   onChange=(
                     event =>
                       event
                       |> Form.valueFromEvent
                       |> (value => value |> onChange(Password))
                   )
                 />
                 <Button type_=Submit>
                   ("Login" |> ReasonReact.string)
                 </Button>
               </form>
           )
      </LoginForm>
    </div>,
};