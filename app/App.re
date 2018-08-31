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

module LazyAdminConfig = {
  module type t = (module type of Admin);
  let request = _ => DynamicImport.(import("./Admin.bs.js") |> resolve);
  let key = "Admin";
};

module LazyAdmin = DynamicResource.Make(LazyAdminConfig);

let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _self =>
    <Placeholder delayMs=300 fallback={ReasonReact.string("Loading...")}>
      <Router.Consumer key="Router">
        ...{
             route =>
               switch (route.path) {
               | ["admin", ..._rest] =>
                 <LazyAdmin render=(((module Admin)) => <Admin />) />
               | ["login"] => <Login />
               | [] => <p> {"Home" |> ReasonReact.string} </p>
               | _ => <NotFound />
               }
           }
      </Router.Consumer>
    </Placeholder>,
};