Css.(
  global(
    "body",
    [margin(0. |. rem), padding(0. |. rem), fontFamily("sans-serif")],
  )
);

let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _self =>
    <StrictMode>
      <Router.Consumer>
        ...(
             route =>
               switch (route.path) {
               | [] => <p> ("Home" |> ReasonReact.string) </p>
               | _ => <p> ("Not found..." |> ReasonReact.string) </p>
               }
           )
      </Router.Consumer>
    </StrictMode>,
};