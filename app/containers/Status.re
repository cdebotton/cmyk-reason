let component = ReasonReact.statelessComponent("Status");

let make = (~code, children) => {
  ...component,
  render: _self =>
    <Router.Consumer>
      ...{
           route =>
             switch (route.staticContext) {
             | None => children
             | Some(context) =>
               code |> Router.(context->statusCodeSet);
               children;
             }
         }
    </Router.Consumer>,
};