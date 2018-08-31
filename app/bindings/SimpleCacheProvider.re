type cache;
[@bs.module "simple-cache-provider"] external createCache: unit => cache = "";

[@bs.module "simple-cache-provider"]
external createResource: ('a => 'b, 'a => string) => unit = "";