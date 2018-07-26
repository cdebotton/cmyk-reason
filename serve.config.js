module.exports = {
  add: (app, middleware) => {
    app.use(async (ctx, next) => {
      ctx.set('Access-Control-Allow-Origin', '*');
      await next();
    });
    middleware.content();
    middleware.webpack();
  },
};
