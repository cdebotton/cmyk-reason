module RouterContext =
  ReactContext.Make({
    type t = ReasonReact.Router.url;
    let debugName = "RouterContext";
    let initialValue = ReasonReact.Router.dangerouslyGetInitialUrl();
  });

module Browser = {
  type state = ReasonReact.Router.url;
  type action =
    | Push(ReasonReact.Router.url);

  let component = ReasonReact.reducerComponent("BrowserRouter");
  let make = children => {
    ...component,
    initialState: () => ReasonReact.Router.dangerouslyGetInitialUrl(),
    reducer: (action, _state) =>
      switch (action) {
      | Push(url) => ReasonReact.Update(url)
      },
    subscriptions: ({send}) => [
      ReasonReact.Sub(
        () => ReasonReact.Router.watchUrl(url => Push(url) |> send),
        ReasonReact.Router.unwatchUrl,
      ),
    ],
    render: ({state}) =>
      <RouterContext.Provider value=state>
        ...children
      </RouterContext.Provider>,
  };
};

module Server = {
  let component = ReasonReact.statelessComponent("ServerRouter");

  let make = (~pathname, children) => {
    ...component,
    render: _self =>
      <RouterContext.Provider value={path: pathname, hash: "", search: ""}>
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

let staticRouterJs =
  ReasonReact.wrapReasonForJs(
    ~component=Server.component,
    jsProps => {
      let pathname = jsProps##pathname |> getPath;
      Server.make(~pathname, jsProps##children);
    },
  );

module Consumer = RouterContext.Consumer;