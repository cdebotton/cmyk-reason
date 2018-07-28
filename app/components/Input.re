module Styles = {
  open Css;

  let label =
    style([
      position(absolute),
      pointerEvents(none),
      userSelect(none),
      fontSize(0.8 |. rem),
    ]);
  let input =
    style([
      borderBottom(1 |> px, solid, black),
      border(0 |. px, none, transparent),
      selector(":focus", [outline(0 |. px, none, transparent)]),
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

let defaultOnChange = _event => ();

module LabelPoseConfig = {
  [@bs.deriving abstract]
  type state = {
    opacity: float,
    y: float,
  };

  [@bs.deriving abstract]
  type t = {
    visible: state,
    hidden: state,
  };

  let config =
    t(
      ~visible=state(~opacity=1., ~y=-20.),
      ~hidden=state(~opacity=0., ~y=0.),
    );

  let element = Pose.Label;

  type pose =
    | Visible
    | Hidden;

  let get =
    fun
    | Visible => "visible"
    | Hidden => "hidden";
};

module Label = Pose.Make(LabelPoseConfig);

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
    <span ?className>
      <Label
        className=Styles.label
        pose=(
          switch (value) {
          | "" => Hidden
          | _ => Visible
          }
        )>
        (placeholder |> ReasonReact.string)
      </Label>
      <input
        className=Styles.input
        ?name
        value
        onChange
        placeholder
        type_=(inputType(type_))
      />
    </span>,
};