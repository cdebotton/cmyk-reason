type resourceType;

[@bs.deriving abstract]
type cache('k, 'v, 'a) =
  pri {
    invalidate: unit => unit,
    read: (resourceType, 'k, 'a => Js.Promise.t('v), 'a) => 'v,
    preload: (resourceType, 'k, 'a => Js.Promise.t('v), 'a) => unit,
    [@bs.as "$$typeof"]
    typeof: int,
  };

[@bs.module "simple-cache-provider"]
external createCache: unit => cache('k, 'v, 'a) = "";

type resource;
[@bs.module "simple-cache-provider"]
external createResource: ('a => 'b, 'a => string) => resource = "";

[@bs.send] external read: (resource, cache('k, 'v, 'a), 'k) => 'v = "";
[@bs.send] external preload: (resource, cache('k, 'v, 'a), 'k) => 'v = "";