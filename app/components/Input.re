module Styles = {
  open Css;

  let colors = Colors.neutral |> Utils.joinList;

  let main =
    style([
      position(relative),
      display(inlineBlock),
      before([
        `declaration(("content", " ")),
        position(absolute),
        display(block),
        top(100. |> pct),
        width(100. |> pct),
        height(2 |> px),
        `declaration((
          "backgroundImage",
          {j|linear-gradient(to right, $colors)|j},
        )),
      ]),
    ]);

  let label =
    style([
      position(absolute),
      pointerEvents(none),
      userSelect(none),
      fontSize(0.6 |> rem),
      fontWeight(600),
      textTransform(uppercase),
      left(0.75 |> rem),
      opacity(0.),
    ]);

  let input =
    style([
      fontSize(1. |> rem),
      padding2(~v=0.5 |> rem, ~h=0.75 |> rem),
      border(0 |> px, none, transparent),
      focus([outline(0 |> px, none, transparent)]),
    ]);
};

let component = ReasonReact.statelessComponent("Input");

type inputType =
  | Text
  | Password
  | Email
  | Checkbox;

let inputType = type_ =>
  switch (type_) {
  | Text => "text"
  | Password => "password"
  | Email => "email"
  | Checkbox => "checkbox"
  };

let defaultOnChange = _value => ();

let make =
    (
      ~placeholder,
      ~value="",
      ~className=?,
      ~name=?,
      ~onChange=defaultOnChange,
      ~type_=Text,
      _children,
    ) => {
  ...component,
  render: _self =>
    <span
      className={
        [Some(Styles.main), className]
        |> Utils.unwrapOptionalList
        |> Utils.joinList(~sep=" ")
      }>
      <Label
        className=Styles.label
        pose={
          switch (value) {
          | "" => Hidden
          | _ => Visible
          }
        }>
        {placeholder |> ReasonReact.string}
      </Label>
      <input
        className=Styles.input
        ?name
        value
        onChange={event => ReactEvent.Form.(event->target##value) |> onChange}
        placeholder
        type_={inputType(type_)}
      />
    </span>,
};