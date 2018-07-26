[@bs.module "react"]
external reactClass : ReasonReact.reactClass = "StrictMode";

let make = children =>
  ReasonReact.wrapJsForReason(~reactClass, ~props=Js.Obj.empty(), children);