module type Config = {
  module type t;
  let key: string;
  let request: _ => Js.Promise.t(module t);
};

module Make = (Config: Config) => {
  open SimpleCacheProvider;

  type t = (module Config.t);
  type renderProp = t => ReasonReact.reactElement;

  let cache: cache(string, t, unit) = SimpleCacheProvider.createCache();

  let resource =
    SimpleCacheProvider.createResource(Config.request, key => key);

  let preload = () => resource->preload(cache, Config.key) |> ignore;

  let component = ReasonReact.statelessComponent("DynamicComponent");

  let make = (~render: renderProp, _children) => {
    ...component,
    render: _self => {
      let data = resource->read(cache, Config.key);
      render(data);
    },
  };
};