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
               | ["admin", "users"] =>
                 <p> ("People" |> ReasonReact.string) </p>
               | _ => <p> ("Uh oh" |> ReasonReact.string) </p>
               }
           )
      </Router.Consumer>
    </div>,
};