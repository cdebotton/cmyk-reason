let component = ReasonReact.statelessComponent("Link");

module Styles = {
  open Css;

  let getColor = active =>
    if (active) {
      color(red);
    } else {
      color(black);
    };

  let main = active => style([textDecoration(none), getColor(active)]);
};

let rec matchPartialPath = ((path, href), ~exact, ~match=true, ()) =>
  switch (path, href, exact, match) {
  | ([a, ...path], [b, ...href], _, true) when a === b =>
    matchPartialPath((path, href), ~exact, ~match=true, ())
  | (_, [], false, true) => true
  | ([], [], true, true) => true
  | (_, _, _, _) => false
  };

let make = (~href, ~exact=false, children) => {
  let onClick = event => {
    event |> ReactEventRe.Mouse.preventDefault;
    href |> ReasonReact.Router.push;
  };

  {
    ...component,
    render: _self =>
      <Router.Consumer>
        ...(
             route => {
               let active =
                 matchPartialPath(
                   (route.path, href |> Router.getPath),
                   ~exact,
                   (),
                 );
               let className = Styles.main(active);
               <a href onClick className> (children |> ReasonReact.array) </a>;
             }
           )
      </Router.Consumer>,
  };
};