let component = ReasonReact.statelessComponent("Status");

let make = (~code, children) => {
  ...component,
  render: _self =>
    <Router.Consumer>
      ...(
           route =>
             switch (route.staticContext) {
             | None => children
             | Some(context) =>
               context |. Router.statusCodeSet(code);
               children;
             }
         )
    </Router.Consumer>,
};