let component = ReasonReact.statelessComponent("Link");

module Styles = {
  open Css;

  let main = style([textDecoration(none), color(black)]);
  let active = style([color(red)]);
};

let rec matchPartialPath = ((path, href), ~exact, ~match=true, ()) =>
  switch (path, href, exact, match) {
  | ([a, ...path], [b, ...href], _, true) when a === b =>
    matchPartialPath((path, href), ~exact, ~match=true, ())
  | (_, [], false, true) => true
  | ([], [], true, true) => true
  | (_, _, _, _) => false
  };

let make =
    (
      ~href,
      ~className=?,
      ~activeClassName=Styles.active,
      ~exact=false,
      children,
    ) => {
  let onClick = event => {
    ReactEvent.Mouse.(event->preventDefault);
    href |> ReasonReact.Router.push;
  };

  {
    ...component,
    render: _self =>
      <Router.Consumer>
        ...{
             route => {
               let active =
                 matchPartialPath(
                   (route.path, href |> Router.getPath),
                   ~exact,
                   (),
                 );

               let whenActiveClassName =
                 if (active) {
                   Some(activeClassName);
                 } else {
                   None;
                 };

               let className =
                 Utils.(
                   [Some(Styles.main), className, whenActiveClassName]
                   |> unwrapOptionalList
                   |> joinList(~sep=" ")
                 );
               <a href onClick className> ...children </a>;
             }
           }
      </Router.Consumer>,
  };
};