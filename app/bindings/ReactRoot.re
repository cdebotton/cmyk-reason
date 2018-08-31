exception Root_not_found(string);

type element;

[@bs.val] [@bs.return nullable] [@bs.scope "document"]
external getElementById: string => option(element) = "";

type root;

[@bs.module "react-dom"] external unstable_createRoot: element => root = "";

[@bs.send] external render: (root, ReasonReact.reactElement) => unit = "";

let create = id => {
  let element = getElementById(id);

  switch (element) {
  | Some(element) => unstable_createRoot(element)
  | None => raise(Root_not_found(id))
  };
};