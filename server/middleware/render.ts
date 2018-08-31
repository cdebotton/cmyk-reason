import { Context } from 'koa';

function render() {
  return (ctx: Context) => {
    if (ctx.req.url !== '/graphql') {
      ctx.body =
        '<!doctype html>' +
        '<html lang="en">' +
        '<head>' +
        '<title>CMYK</title>' +
        '<link href="https://fonts.googleapis.com/css?family=Oswald:500|Roboto:400,400i" rel="stylesheet">' +
        '</head>' +
        '<body>' +
        '<main id="app">' +
        '</main>' +
        '<script src="http://localhost:8080/vendors~main.bundle.js"></script>' +
        '<script src="http://localhost:8080/bundle.js"></script>' +
        '</body>' +
        '</html>';
    }
  };
}

export default render;
