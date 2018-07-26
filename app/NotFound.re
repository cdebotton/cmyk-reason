let component = ReasonReact.statelessComponent("NotFound");

module Styles = {
  open Css;
  let pathBlock = style([display(inlineBlock)]);
};

let rec stringFromPath = (path, ~acc="", ()) =>
  switch (path) {
  | [part, ...path] => stringFromPath(path, ~acc={j|$acc/$part|j}, ())
  | [] => acc
  };

let make = _children => {
  ...component,
  render: _self =>
    <Status code=404>
      <h1> ("Uh oh!" |> ReasonReact.string) </h1>
      <div>
        <p>
          ("I can't find " |> ReasonReact.string)
          <Router.Consumer>
            ...(
                 ({path}) =>
                   <pre className=Styles.pathBlock>
                     (path |> stringFromPath(_, ()) |> ReasonReact.string)
                   </pre>
               )
          </Router.Consumer>
          ("." |> ReasonReact.string)
        </p>
      </div>
    </Status>,
};