module Styles = {
  open Css;

  let button = (format: Colors.t) => {
    let bgColor =
      switch (format) {
      | Light
      | Dark
      | Neutral => Colors.getColor(Neutral)
      | Positive => Colors.getColor(Positive)
      | Negative => Colors.getColor(Negative)
      };

    let fgColor =
      switch (format) {
      | Light
      | Dark
      | Neutral
      | Negative => Colors.getColor(Light)
      | Positive => Colors.getColor(Dark)
      };

    let buttonBackgroundColors =
      [bgColor(0), bgColor(1), bgColor(2), bgColor(3)] |> Utils.joinList;

    let textColor = fgColor(1);

    style([
      fontSize(0.75 |. rem),
      padding2(~v=0.5 |. rem, ~h=0.8 |. rem),
      border(0 |> px, none, transparent),
      `declaration(("color", textColor)),
      cursor(`pointer),
      `declaration((
        "backgroundImage",
        {j|linear-gradient(to bottom right, $buttonBackgroundColors)|j},
      )),
    ]);
  };
};

let component = ReasonReact.statelessComponent("Button");

type buttonType =
  | Button
  | Submit
  | Reset;

let getButtonType = type_ =>
  switch (type_) {
  | Button => "button"
  | Submit => "submit"
  | Reset => "reset"
  };

let defaultOnClick = _event => ();

let make =
    (
      ~type_=Button,
      ~className=?,
      ~format: Colors.t=Neutral,
      ~onClick=defaultOnClick,
      children,
    ) => {
  ...component,
  render: _self =>
    <button
      className=Utils.(
                  [Some(format |> Styles.button), className]
                  |> unwrapOptionalList
                  |> joinList(~sep=" ")
                )
      onClick
      type_=(getButtonType(type_))>
      (children |> ReasonReact.array)
    </button>,
};