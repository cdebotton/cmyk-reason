declare module 'glamor/server' {
  interface RenderStaticResponse<T> {
    html: T;
    css: string;
    ids: string[];
    rules: string[];
  }
  export function renderStatic<T extends NodeJS.ReadableStream | string>(
    callback: () => T,
  ): RenderStaticResponse<T>;
}
