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

module Admin = {
  let component = ReasonReact.statelessComponent("Admin");
  let make = _children => {
    ...component,
    render: _self =>
      <div>
        <h1> ("Admin" |> ReasonReact.string) </h1>
        <Link exact=true href="/admin">
          ("Dashboard" |> ReasonReact.string)
        </Link>
        <Link href="/admin/documents">
          ("Documents" |> ReasonReact.string)
        </Link>
        <Link href="/admin/users"> ("Users" |> ReasonReact.string) </Link>
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
      </div>,
  };
};

let component = ReasonReact.statelessComponent("AdminProtect");

let make = _children => {
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
             | Data(_response) => <Admin />
             }
         )
    </SessionQuery>,
};