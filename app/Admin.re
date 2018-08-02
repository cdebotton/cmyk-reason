module Session = [%graphql
  {|
  query Session {
    session {
      token
      user {
        id
        email
      }
    }
  }
  |}
];
module SessionQuery = ReasonApollo.CreateQuery(Session);

module LogoutButton = {
  [@bs.send]
  external resetStore : (ApolloClient.generatedApolloClient, unit) => unit =
    "";

  module Logout = [%graphql {|
    mutation Logout {
      logout
    }
  |}];

  module LogoutMutation = ReasonApollo.CreateMutation(Logout);

  let logout = (mutate, apolloClient) =>
    Js.Promise.(mutate() |> (_data => apolloClient |. resetStore() |> resolve))
    |> ignore;

  let component = ReasonReact.statelessComponent("LogoutButton");

  let make = children => {
    ...component,
    render: _self =>
      <ApolloConsumer>
        ...(
             apolloClient =>
               <LogoutMutation>
                 ...(
                      (mutate, _renderPropObj) =>
                        <Button
                          type_=Button
                          onClick=(_event => logout(mutate, apolloClient))>
                          ...children
                        </Button>
                    )
               </LogoutMutation>
           )
      </ApolloConsumer>,
  };
};

module AdminContainer = {
  let component = ReasonReact.statelessComponent("AdminProtect");

  let make = children => {
    ...component,
    render: _self =>
      <SessionQuery>
        ...(
             ({result}) =>
               switch (result) {
               | Loading => ReasonReact.null
               | Error(_err) => ReasonReact.null
               | Data(response) when response##session === None =>
                 <Redirect to_="/login" />
               | Data(_response) => children |> ReasonReact.array
               }
           )
      </SessionQuery>,
  };
};

let component = ReasonReact.statelessComponent("Admin");
let make = _children => {
  ...component,
  render: _self =>
    <AdminContainer>
      <div>
        <h1> ("Admin" |> ReasonReact.string) </h1>
        <LogoutButton>
          <FontAwesomeIcon icon=FreeSolidIcons.faSignOutAlt />
        </LogoutButton>
        <Link exact=true href="/admin">
          <FontAwesomeIcon icon=FreeSolidIcons.faHome />
        </Link>
        <Link href="/admin/documents">
          <FontAwesomeIcon icon=FreeSolidIcons.faFolderOpen />
        </Link>
        <Link href="/admin/users">
          <FontAwesomeIcon icon=FreeSolidIcons.faUser />
        </Link>
        <Router.Consumer>
          ...(
               ({path}) =>
                 switch (path) {
                 | ["admin"] => <p> ("Dashboard" |> ReasonReact.string) </p>
                 | ["admin", "documents"] =>
                   <p> ("Docs" |> ReasonReact.string) </p>
                 | ["admin", "users", ..._rest] => <AdminUsers />
                 | _ => <NotFound />
                 }
             )
        </Router.Consumer>
      </div>
    </AdminContainer>,
};