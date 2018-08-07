[@bs.deriving abstract]
type staticContext = {
  mutable statusCode: int,
  mutable url: option(string),
};

let url = ReasonReact.Router.dangerouslyGetInitialUrl();

type router = {
  path: list(string),
  hash: string,
  search: string,
  staticContext: option(staticContext),
};

module RouterConfig = {
  type t = router;
  let debugName = "RouterContext";
  let value = {
    path: url.path,
    search: url.search,
    hash: url.hash,
    staticContext: None,
  };
};

module RouterContext = ReactContext.Make(RouterConfig);

module Browser = {
  type state = router;
  type action =
    | Push(ReasonReact.Router.url);

  let component = ReasonReact.reducerComponent("BrowserRouter");
  let make = children => {
    ...component,
    initialState: () => {
      path: url.path,
      hash: url.hash,
      search: url.search,
      staticContext: None,
    },
    reducer: (action, state) =>
      switch (action) {
      | Push(url) =>
        ReasonReact.Update({
          ...state,
          path: url.path,
          hash: url.hash,
          search: url.search,
        })
      },
    didMount: ({send, onUnmount}) => {
      let urlWatcher = ReasonReact.Router.watchUrl(url => Push(url) |> send);

      onUnmount(() => ReasonReact.Router.unwatchUrl(urlWatcher));
    },
    render: ({state}) =>
      <RouterContext.Provider value=state>
        ...children
      </RouterContext.Provider>,
  };
};

let getPath = pathname =>
  switch (pathname) {
  | ""
  | "/" => []
  | raw =>
    let raw = Js.String.sliceToEnd(~from=1, raw);
    let raw =
      switch (Js.String.get(raw, Js.String.length(raw) - 1)) {
      | "/" => Js.String.slice(~from=0, ~to_=1, raw)
      | _ => raw
      };
    raw |> Js.String.split("/") |> Utils.arrayToList;
  };

module Server = {
  let component = ReasonReact.statelessComponent("ServerRouter");

  let make = (~pathname, ~staticContext, children) => {
    ...component,
    render: _self =>
      <RouterContext.Provider
        value={
          path: pathname,
          hash: "",
          search: "",
          staticContext: Some(staticContext),
        }>
        ...children
      </RouterContext.Provider>,
  };
};

let staticRouterJs =
  ReasonReact.wrapReasonForJs(
    ~component=Server.component,
    jsProps => {
      let pathname = jsProps##pathname |> getPath;
      Server.make(
        ~pathname,
        ~staticContext=jsProps##context,
        jsProps##children,
      );
    },
  );

module Consumer = RouterContext.Consumer;