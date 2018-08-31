[@bs.module "react"]
external reactClass: ReasonReact.reactClass = "Placeholder";

let make = (~delayMs=0, ~fallback=ReasonReact.null, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass,
    ~props={"delayMs": delayMs, "fallback": fallback},
    children,
  );