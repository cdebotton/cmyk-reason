open SimpleCacheProvider;

let cache = SimpleCacheProvider.createCache();

module type Config = {
  module type t;
  let key: string;
  let request: _ => Js.Promise.t(module t);
};

module Make = (Config: Config) => {
  let resource =
    SimpleCacheProvider.createResource(Config.request, key => key);

  type renderProp = (module Config.t) => ReasonReact.reactElement;
  let component = ReasonReact.statelessComponent("DynamicComponent");

  let make = (~render: renderProp, _children) => {
    ...component,
    render: _self => {
      let data = resource->read(cache, Config.key);
      render(data);
    },
  };
};