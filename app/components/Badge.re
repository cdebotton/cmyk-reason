module Styles = {
  let badge =
    Css.(
      style([
        borderRadius(3 |. px),
        backgroundColor(black),
        color(white),
        fontSize(0.75 |. rem),
        padding(0.25 |. rem),
      ])
    );
};

let component = ReasonReact.statelessComponent("Badge");

let make = children => {
  ...component,
  render: _self => <span className=Styles.badge> ...children </span>,
};