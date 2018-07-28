module Styles = {
  open Css;
  let button =
    style([border(0 |> px, none, transparent), cursor(`pointer)]);
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

let make = (~type_=Button, ~onClick=defaultOnClick, children) => {
  ...component,
  render: _self =>
    <button className=Styles.button onClick type_=(getButtonType(type_))>
      (children |> ReasonReact.array)
    </button>,
};