# WebGPU Game of Life (TypeScript)

GPU‑accelerated Conway’s Game of Life built with **TypeScript + WebGPU**, based on Google’s [Your First WebGPU App codelab](https://codelabs.developers.google.com/your-first-webgpu-app) .

---

## 🧰 Installation & Setup

Install required developer dependencies:

```bash
npm install --save-dev @webgpu/types vite-plugin-glsl vite typescript
````

* **@webgpu/types\@0.1.64** — adds TypeScript types for the WebGPU API ([npm][1])
* **vite\@7.0.6** — fast development server and build tool
* **vite-plugin-glsl\@1.5.1** — imports `.wgsl` shader files as strings, supports HMR ([npm][2])

Add this to your `tsconfig.json` to enable type checking:

```json
{
  "compilerOptions": {
    "types": ["@webgpu/types", "vite-plugin-glsl/ext"]
  }
}
```

---

## ⚙️ Development & Production

Clone the repository, install dependencies, and launch:

```bash
//clone the repo in your device
cd <project-folder>
npm install
npm run dev
```

* Starts Vite dev server with hot module reloading — including live WGSL shader updates via `vite-plugin-glsl`.

To build and preview production artifacts:

```bash
npm run build
npm run preview
```

* `build` compiles optimized assets into `dist/`
* `preview` serves the production bundle locally (default: `http://localhost:4173`)

---

## 🌐 Browser Requirements

WebGPU support is required:

* Works in **Chrome / Edge version ≥ 113** with the flag `chrome://flags/#enable-unsafe-webgpu` enabled, if needed ([npm][2], [npm][1], [en.wikipedia.org][3])
* Experimental support in **Firefox Nightly** and **Safari Tech Preview** — check recent status

---

## 📚 Reference

Project follows the structure and logic of Google’s official **WebGPU Game of Life codelab**, which teaches how to set up WebGPU, use compute shaders for simulation, and render using vertex and fragment shaders ([Google Codelabs][4]).

---

## ✅ Summary

* Install packages: `@webgpu/types`, `vite`, `vite-plugin-glsl`, `typescript`
* Develop: `npm run dev`
* Build: `npm run build`
* Preview: `npm run preview`
* Ensure your browser supports WebGPU API

Enjoy experimenting with GPU-based simulations and shader logic!

[1]: https://www.npmjs.com/package/%40webgpu/types?utm_source=chatgpt.com "webgpu/types"
[2]: https://www.npmjs.com/package/vite-plugin-glsl?utm_source=chatgpt.com "vite-plugin-glsl"
[3]: https://en.wikipedia.org/wiki/WebGPU?utm_source=chatgpt.com "WebGPU"
[4]: https://codelabs.developers.google.com/your-first-webgpu-app?utm_source=chatgpt.com "Your first WebGPU app"
