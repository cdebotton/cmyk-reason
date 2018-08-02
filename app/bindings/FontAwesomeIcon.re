type icon;
type flip =
  | Horizontal
  | Vertical
  | Both;
type size =
  | ExtraSmall
  | Small
  | Large
  | OneTimes
  | TwoTimes
  | ThreeTimes
  | FourTimes
  | FiveTimes
  | SixTimes
  | SevenTimes
  | EightTimes
  | NineTimes
  | TenTimes;
type pull =
  | Left
  | Right;
type rotation =
  | Ninety
  | OneHundredEight
  | TwoHundredSeventy;

let getFlip = flip =>
  switch (flip) {
  | Some(flip) when flip === Horizontal => Some("horizontal")
  | Some(flip) when flip === Vertical => Some("vertical")
  | Some(flip) when flip === Both => Some("both")
  | _ => None
  };

let getSize = size =>
  switch (size) {
  | Some(size) when size === ExtraSmall => Some("xs")
  | Some(size) when size === Small => Some("sm")
  | Some(size) when size === Large => Some("lg")
  | Some(size) when size === OneTimes => Some("1x")
  | Some(size) when size === TwoTimes => Some("2x")
  | Some(size) when size === ThreeTimes => Some("3x")
  | Some(size) when size === FourTimes => Some("4x")
  | Some(size) when size === FiveTimes => Some("5x")
  | Some(size) when size === SixTimes => Some("6x")
  | Some(size) when size === SevenTimes => Some("7x")
  | Some(size) when size === EightTimes => Some("8x")
  | Some(size) when size === NineTimes => Some("9x")
  | Some(size) when size === TenTimes => Some("10x")
  | _ => None
  };

let getPull = pull =>
  switch (pull) {
  | Some(pull) when pull === Left => Some("left")
  | Some(pull) when pull === Right => Some("right")
  | _ => None
  };

let getRotation = rotation =>
  switch (rotation) {
  | Some(rotation) when rotation === Ninety => Some(90)
  | Some(rotation) when rotation === OneHundredEight => Some(180)
  | Some(rotation) when rotation === TwoHundredSeventy => Some(270)
  | _ => None
  };

[@bs.module "@fortawesome/react-fontawesome"]
external reactClass : ReasonReact.reactClass = "FontAwesomeIcon";

let make =
    (
      ~icon: icon,
      ~mask: option(icon)=?,
      ~className: option(string)=?,
      ~color: option(string)=?,
      ~spin: option(bool)=?,
      ~pulse: option(bool)=?,
      ~border: option(bool)=?,
      ~fixedWidth: option(bool)=?,
      ~inverse: option(bool)=?,
      ~listItem: option(bool)=?,
      ~flip: option(flip)=?,
      ~size: option(size)=?,
      ~pull: option(pull)=?,
      ~rotation: option(rotation)=?,
      ~style: option(ReactDOMRe.Style.t)=?,
      _children,
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass,
    ~props={
      "icon": icon,
      "mask": mask |> Js.Undefined.fromOption,
      "className": className |> Js.Undefined.fromOption,
      "color": color |> Js.Undefined.fromOption,
      "spin": spin |> Js.Undefined.fromOption,
      "pulse": pulse |> Js.Undefined.fromOption,
      "border": border |> Js.Undefined.fromOption,
      "fixedWidth": fixedWidth |> Js.Undefined.fromOption,
      "inverse": inverse |> Js.Undefined.fromOption,
      "listItem": listItem |> Js.Undefined.fromOption,
      "flip": flip |> getFlip |> Js.Undefined.fromOption,
      "size": size |> getSize |> Js.Undefined.fromOption,
      "pull": pull |> getPull |> Js.Undefined.fromOption,
      "rotation": rotation |> getRotation |> Js.Undefined.fromOption,
      "style": style |> Js.Undefined.fromOption,
    },
    [||],
  );

/**
  transform?: string | Transform
  symbol?: FaSymbol

  export type FaSymbol = string | boolean;
  export interface Transform {
    size?: number;
    x?: number;
    y?: number;
    rotate?: number;
    flipX?: boolean;
    flipY?: boolean;
  }
*/;