module Styles = {
  open Css;

  let bgColor = Colors.getColor(Light);
  let sidebarBgColor = [bgColor(0), bgColor(1)] |> Utils.joinList;
  let container =
    style([display(grid), gridTemplateColumns([minContent, auto])]);
  let heading =
    style([
      `declaration(("writingMode", "vertical-lr")),
      margin(0 |. px),
      padding(0.4 |. rem),
      userSelect(none),
    ]);
  let content = style([padding2(~v=0.5 |. rem, ~h=1. |. rem)]);
  let sidebar =
    style([
      height(100. |. vh),
      display(`flex),
      flexDirection(column),
      padding2(~v=0.5 |. rem, ~h=0. |. rem),
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

let component = ReasonReact.statelessComponent("Admin");
let make = _children => {
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

  let admin =
    <div className=Styles.container>
      <div className=Styles.sidebar>
        <Heading className=Styles.heading level=1>
          ("CMYK" |> ReasonReact.string)
        </Heading>
        <Link className=Styles.adminLink exact=true href="/admin">
          ...<FontAwesomeIcon icon=SolidIcons.faHome />
        </Link>
        <Link className=Styles.adminLink href="/admin/documents">
          ...<FontAwesomeIcon icon=SolidIcons.faFolderOpen />
        </Link>
        <Link className=Styles.adminLink href="/admin/users">
          ...<FontAwesomeIcon icon=SolidIcons.faUser />
        </Link>
        <LogoutButton>
          ...<FontAwesomeIcon icon=SolidIcons.faSignOutAlt />
        </LogoutButton>
      </div>
      <div className=Styles.content>
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
    </div>;
  {
    ...component,
    render: _self =>
      <SessionQuery>
        ...(
             ({result}) =>
               switch (result) {
               | Loading => <Loader />
               | Error(_err) => ReasonReact.null
               | Data(response) when response##session === None =>
                 <Redirect to_="/login" />
               | Data(_response) => admin
               }
           )
      </SessionQuery>,
  };
};