Css.(
  global(
    "body",
    [
      margin(0. |> rem),
      padding(0. |> rem),
      fontFamily("sans-serif"),
      fontFamily("Roboto, sans-serif"),
    ],
  ),
  global("*,*::before,*::after", [boxSizing(borderBox)]),
);

module Styles = {
  open Css;
  let main =
    style([
      gridTemplateColumns([auto, minContent, maxContent]),
      width(100. |> vw),
      minHeight(100. |> vh),
      overflow(auto),
    ]);
};

let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _self =>
    <Router.Consumer key="Router">
      ...{
           route =>
             switch (route.path) {
             | ["admin", ..._rest] => <Admin />
             | ["login"] => <Login />
             | [] => <p> {"Home" |> ReasonReact.string} </p>
             | _ => <NotFound />
             }
         }
    </Router.Consumer>,
};