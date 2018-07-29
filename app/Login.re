module Authenticate = [%graphql
  {|
  mutation Authenticate($input: AuthenticateInput!) {
    authenticate(input: $input) {
      token
    }
  }
  |}
];

module AuthenticateMutation = ReasonApollo.CreateMutation(Authenticate);

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
    style([
      width(100. |. vw),
      minHeight(100. |. vh),
      overflow(auto),
      display(grid),
      alignContent(center),
      justifyContent(center),
    ]);

  let title = style([paddingLeft(0.75 |. rem)]);

  let form =
    style([
      maxWidth(44. |. rem),
      display(grid),
      gridGap(1. |. rem),
      gridTemplateColumns([1. |. fr, 1. |. fr]),
      gridTemplateRows([minContent, minContent, minContent]),
    ]);

  let inputEmail = style([gridRow(2, 3), gridColumn(1, 2)]);
  let inputPassword = style([gridRow(2, 3), gridColumn(2, 3)]);
  let loginButton = style([gridRow(3, 4), gridColumn(2, 3)]);
};

let component = ReasonReact.statelessComponent("Login");

let make = _children => {
  ...component,
  render: _self =>
    <div className=Styles.page>
      <AuthenticateMutation>
        ...(
             (mutate, _responseObject) =>
               <LoginForm
                 initialValues={email: "", password: ""}
                 onSubmit=(
                   ({email, password}) => {
                     let authenticate =
                       Authenticate.make(
                         ~input={"email": email, "password": password},
                         (),
                       );

                     mutate(~variables=authenticate##variables, ()) |> ignore;
                   }
                 )>
                 ...(
                      ({getValue, onChange, handleSubmit}) =>
                        <form className=Styles.form onSubmit=handleSubmit>
                          <Heading className=Styles.title level=2>
                            ("Login" |> ReasonReact.string)
                          </Heading>
                          <Input
                            className=Styles.inputEmail
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
                            className=Styles.inputPassword
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
                          <Button
                            format=Neutral
                            className=Styles.loginButton
                            type_=Submit>
                            ("Login" |> ReasonReact.string)
                          </Button>
                        </form>
                    )
               </LoginForm>
           )
      </AuthenticateMutation>
    </div>,
};