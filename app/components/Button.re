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

    let buttonBackgroundColors = [bgColor(0), bgColor(3)] |> Utils.joinList;

    let textColor = fgColor(1);

    style([
      borderRadius(3 |. px),
      fontSize(0.75 |. rem),
      padding2(~v=0.5 |. rem, ~h=0.8 |. rem),
      border(0 |> px, none, transparent),
      `declaration(("color", textColor)),
      cursor(`pointer),
      boxShadow(~x=0 |. px, ~y=1 |. px, ~blur=2 |. px, hsla(0, 0, 0, 0.45)),
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
  let className =
    Utils.(
      [Some(format |> Styles.button), className]
      |> unwrapOptionalList
      |> joinList(~sep=" ")
    );

  let type_ = getButtonType(type_);

  {
    ...component,
    render: _self => <button className onClick type_> ...children </button>,
  };
};