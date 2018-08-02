module Styles = {
  open Css;

  let bgColor = Colors.getColor(Light);
  let sidebarBgColor =
    [bgColor(0), bgColor(1), bgColor(2), bgColor(3)] |> Utils.joinList;

  let container =
    style([display(grid), gridTemplateColumns([minContent, auto])]);
  let heading =
    style([
      `declaration(("writingMode", "vertical-lr")),
      margin(0 |. px),
      padding(0.4 |. rem),
      userSelect(none),
    ]);
  let sidebar =
    style([
      height(100. |. vh),
      display(`flex),
      flexDirection(column),
      flexWrap(nowrap),
      alignItems(center),
      `declaration((
        "backgroundImage",
        {j|linear-gradient(to bottom right, $sidebarBgColor)|j},
      )),
    ]);
  let logoutButton = style([marginTop(auto)]);
  let adminLink = style([padding(0.5 |. rem)]);
};

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
                          className=Styles.logoutButton
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
      <div className=Styles.container>
        <div className=Styles.sidebar>
          <Heading className=Styles.heading level=1>
            ("Admin" |> ReasonReact.string)
          </Heading>
          <Link className=Styles.adminLink exact=true href="/admin">
            <FontAwesomeIcon icon=SolidIcons.faHome />
          </Link>
          <Link className=Styles.adminLink href="/admin/documents">
            <FontAwesomeIcon icon=SolidIcons.faFolderOpen />
          </Link>
          <Link className=Styles.adminLink href="/admin/users">
            <FontAwesomeIcon icon=SolidIcons.faUser />
          </Link>
          <LogoutButton>
            <FontAwesomeIcon icon=SolidIcons.faSignOutAlt />
          </LogoutButton>
        </div>
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