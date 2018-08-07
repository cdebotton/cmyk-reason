/**
 * 💅 Styles 💅
 */
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

/**
 * 🚀 GraphQL 🚀
 */
module Session = [%graphql
  {|
  query Session {
    session {
      token
    }
  }
  |}
];

module SessionQuery = ReasonApollo.CreateQuery(Session);

module Authenticate = [%graphql
  {|
  mutation Authenticate($input: AuthenticateInput!) {
    authenticate(input: $input)
  }
  |}
];

module AuthenticateMutation = ReasonApollo.CreateMutation(Authenticate);

[@bs.send]
external resetStore : (ApolloClient.generatedApolloClient, unit) => unit = "";

/**
 * ⌨️ Form configuration ⌨️
 */
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

let onSubmit =
    (
      mutate: AuthenticateMutation.apolloMutation,
      apolloClient: ApolloClient.generatedApolloClient,
      {email, password}: LoginConfig.t,
    ) => {
  let authenticate =
    Authenticate.make(~input={"email": email, "password": password}, ());

  Js.Promise.(
    mutate(~variables=authenticate##variables, ())
    |> then_(_data => apolloClient |. resetStore() |> resolve)
    |> ignore
  );
};

/**
 * Component lives here
 */

let component = ReasonReact.statelessComponent("Login");

module LoginContainer = {
  let component = ReasonReact.statelessComponent("LoginContainer");
  let make = children => {
    ...component,
    render: _self =>
      <SessionQuery>
        ...(
             ({result}) =>
               switch (result) {
               | Loading => ReasonReact.null
               | Error(_err) => ReasonReact.null
               | Data(response) when response##session !== None =>
                 <Redirect to_="/admin" />
               | Data(_response) =>
                 <ApolloConsumer>
                   ...(
                        apolloClient =>
                          <AuthenticateMutation>
                            ...(
                                 (mutate, _renderPropObj) =>
                                   children(mutate, apolloClient)
                               )
                          </AuthenticateMutation>
                      )
                 </ApolloConsumer>
               }
           )
      </SessionQuery>,
  };
};

let make = _children => {
  ...component,
  render: _self =>
    <div className=Styles.page>
      <LoginContainer>
        ...(
             (mutate, apolloClient) =>
               <LoginForm
                 initialValues={email: "", password: ""}
                 onSubmit=(onSubmit(mutate, apolloClient))>
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
                            onChange=(onChange(Email))
                          />
                          <Input
                            className=Styles.inputPassword
                            placeholder="Password"
                            type_=Password
                            value=(getValue(Password))
                            name="password"
                            onChange=(onChange(Password))
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
      </LoginContainer>
    </div>,
};