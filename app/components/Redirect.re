let component = ReasonReact.statelessComponent("Redirect");

let make = (~to_, _children) => {
  ...component,
  didMount: _self => ReasonReact.Router.push(to_) |> ignore,
  render: _self =>
    <Router.Consumer>
      ...(
           ({staticContext}) => {
             switch (staticContext) {
             | None => ignore()
             | Some(context) => context.url = Some(to_)
             };
             ReasonReact.null;
           }
         )
    </Router.Consumer>,
};