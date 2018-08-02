type icon;

[@bs.module "@fortawesome/react-fontawesome"]
external reactClass : ReasonReact.reactClass = "FontAwesomeIcon";

let make = (~icon: icon, _children) =>
  ReasonReact.wrapJsForReason(~reactClass, ~props={"icon": icon}, [||]);