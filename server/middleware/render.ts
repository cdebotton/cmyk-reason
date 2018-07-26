import { renderToNodeStream } from 'react-dom/server';
import { Context } from 'koa';
import { renderStatic } from 'glamor/server';

type Renderer = (ctx: Context) => Promise<[JSX.Element, string]>;

function render(render: Renderer) {
  return async (ctx: Context) => {
    const [app, state] = await render(ctx);
    const { html, css } = renderStatic(() => renderToNodeStream(app));

    ctx.respond = false;
    ctx.status = 200;
    ctx.res.write(
      '<!doctype html>' +
        '<html lang="en">' +
        '<head>' +
        '<title>CMYK</title>' +
        `<style>${css}</style>` +
        '</head>' +
        '<body>' +
        '<main id="app">',
    );

    html
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
