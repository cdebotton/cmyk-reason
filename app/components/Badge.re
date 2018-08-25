type format =
  | Positive
  | Neutral
  | Negative;

module Styles = {
  let badge = format => {
    let bgColor =
      switch (format) {
      | Neutral => Colors.getColor(Neutral, 0)
      | Positive => Colors.getColor(Positive, 0)
      | Negative => Colors.getColor(Negative, 0)
      };
    Css.(
      style([
        borderRadius(3 |> px),
        `declaration(("backgroundColor", bgColor)),
        fontWeight(600),
        color(white),
        fontSize(0.75 |> rem),
        padding(0.25 |> rem),
        boxShadow(
          ~x=0 |> px,
          ~y=1 |> px,
          ~blur=2 |> px,
          hsla(0, 0, 0, 0.45),
        ),
      ])
    );
  };
};
let component = ReasonReact.statelessComponent("Badge");

let make = (~className=?, ~format=Neutral, children) => {
  ...component,
  render: _self =>
    <span
      className={
        [Some(Styles.badge(format)), className]
        |> Utils.unwrapOptionalList
        |> Utils.joinList(~sep=" ")
      }>
      ...children
    </span>,
};