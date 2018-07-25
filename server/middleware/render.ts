import { renderToNodeStream } from 'react-dom/server';
import { Context } from 'koa';

type Renderer = (ctx: Context) => Promise<[JSX.Element, string]>;

function render(render: Renderer) {
  return async (ctx: Context) => {
    ctx.respond = false;
    ctx.status = 200;
    ctx.res.write(
      '<!doctype html>' +
        '<html lang="en">' +
        '<head>' +
        '<title>CMYK</title>' +
        '</head>' +
        '<body>' +
        '<main id="app">',
    );

    const [app, state] = await render(ctx);

    renderToNodeStream(app)
      .on('end', () => {
        ctx.res.end(
          '</main>' +
            `<script>window.__APOLLO_STATE__ = ${state};</script>` +
            '<script src="http://localhost:8080/bundle.js"></script>' +
            '</body>' +
            '</html>',
        );
      })
      .pipe(ctx.res);
  };
}

export default render;
