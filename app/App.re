Css.(
  global(
    "body",
    [margin(0. |. rem), padding(0. |. rem), fontFamily("sans-serif")],
  )
);

module Styles = {
  open Css;
  let main = style([gridTemplateColumns([auto, minContent, maxContent])]);
};

let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _self =>
    <StrictMode>
      <div className=Styles.main>
        <Router.Consumer>
          ...(
               route =>
                 switch (route.path) {
                 | ["admin", ..._rest] => <Admin />
                 | [] => <p> ("Home" |> ReasonReact.string) </p>
                 | _ => <p> ("Not found..." |> ReasonReact.string) </p>
                 }
             )
        </Router.Consumer>
      </div>
    </StrictMode>,
};