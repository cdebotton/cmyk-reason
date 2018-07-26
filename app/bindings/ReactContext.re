[@bs.module "react"]
external createContext : 'a => Js.Dict.t(ReasonReact.reactClass) = "";

module type Configuration = {type t; let value: t; let debugName: string;};

module Make = (C: Configuration) => {
  let context = createContext(C.value);

  module Provider = {
    let make = (~value: C.t, children) =>
      ReasonReact.wrapJsForReason(
        ~reactClass=Js.Dict.unsafeGet(context, "Provider"),
        ~props={"value": value},
        children,
      );
  };

  module Consumer = {
    let make = (children: C.t => 'a) =>
      ReasonReact.wrapJsForReason(
        ~reactClass=Js.Dict.unsafeGet(context, "Consumer"),
        ~props=Js.Obj.empty(),
        children,
      );
  };
};