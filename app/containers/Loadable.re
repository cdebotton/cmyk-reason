/* let imported: ref(list(string)) = ref([]);

   module type Config = {module type t; let debugName: string;};

   module Make = (Config: Config) => {
     type state =
       | Loading
       | Error(string)
       | Ready((module Config.t));
     let component = ReasonReact.reducerComponent(Config.debugName);
     let make = (~onLoading, ~onError, ~onReady, ~fetch, ~delay=0, _children) => {
       ...component,
       initialState: () => Loading,
       reducer: (action, _state) =>
         switch (action) {
         | Loading => ReasonReact.Update(Loading)
         | Error(err) => ReasonReact.Update(Error(err))
         | Ready(data) => ReasonReact.Update(Ready(data))
         },
       didMount: _self => imported := ["foo", ...imported^],
       subscriptions: ({send}) => [
         ReasonReact.Sub(
           () =>
             Js.Global.setTimeout(
               () =>
                 DynamicImport.(
                   fetch()
                   |> resolve
                   <$> (data => Ready(data) |> send)
                   <$!> (err => Error(err |> Js.String.make) |> send)
                   |> ignore
                 ),
               delay,
             ),
           Js.Global.clearTimeout,
         ),
       ],
       render: ({state}) =>
         switch (state) {
         | Loading => onLoading()
         | Error(err) => onError(err)
         | Ready(data) => onReady(data)
         },
     };
   }; */