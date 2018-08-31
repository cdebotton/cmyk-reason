type element;
type root;

[@bs.send] external render: (root, ReasonReact.reactElement) => unit = "";
let create: string => root;