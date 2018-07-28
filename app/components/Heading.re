module Styles = {
  open Css;
  let main = style([]);
};

let component = ReasonReact.statelessComponent("Heading");

let make = (~level: int, children) => {
  ...component,
  render: _self => {
    switch (level) {
    | raw when raw < 1 || raw > 6 =>
      Js.Exn.raiseError("Level must be between 1 and 6")
    | _ => ignore()
    };
    ReasonReact.createDomElement(
      {j|h$level|j},
      ~props={"className": Styles.main},
      children,
    );
  },
};