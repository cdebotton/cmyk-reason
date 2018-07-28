type reactPose;

[@bs.module "react-pose"] external pose : reactPose = "default";
[@bs.send] external div : (reactPose, 'a) => ReasonReact.reactClass = "";
[@bs.send] external span : (reactPose, 'a) => ReasonReact.reactClass = "";
[@bs.send] external label : (reactPose, 'a) => ReasonReact.reactClass = "";

type elementType =
  | Div
  | Span
  | Label;

module type Config = {
  type t;
  type pose;
  type state;
  let element: elementType;
  let config: t;
  let get: pose => string;
};

module Make = (C: Config) => {
  let reactClass =
    switch (C.element) {
    | Div => pose |. div(C.config)
    | Span => pose |. span(C.config)
    | Label => pose |. label(C.config)
    };

  let make = (~pose, ~className=?, ~initialPose=?, children) => {
    Js.log(pose |> C.get);
    ReasonReact.wrapJsForReason(
      ~reactClass,
      ~props={
        "pose": pose |> C.get,
        "initialPose": Js.Undefined.fromOption(initialPose),
        "className": Js.Undefined.fromOption(className),
      },
      children,
    );
  };
};