[@bs.module "react"]
external reactClass: ReasonReact.reactClass = "Placeholder";

let make = (~delay=0, ~placeholder=ReasonReact.null, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass,
    ~props={"delay": delay, "placeholder": placeholder},
    children,
  );