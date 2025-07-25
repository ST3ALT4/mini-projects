import shaderCode from "./shader.wgsl"; // Assume a bundler plugin handles WGSL imports
import computeCode from "./compute.wgsl";

const GRID_SIZE = 32;
const UPDATE_INTERVAL = 250;
const WORKGROUP_SIZE = 8;

const canvas = document.querySelector("canvas") as HTMLCanvasElement;
if (!canvas) throw new Error("Canvas not found");

async function init() {
  // WebGPU device initialization
  if (!navigator.gpu) {
    throw new Error("WebGPU not supported on this browser.");
  }

  const adapter = await navigator.gpu.requestAdapter();
  if (!adapter) {
    throw new Error("No appropriate GPUAdapter found.");
  }

  const device = await adapter.requestDevice();

  // Canvas configuration
  const context = canvas.getContext("webgpu") as GPUCanvasContext;
  const canvasFormat = navigator.gpu.getPreferredCanvasFormat();

  context.configure({
    device,
    format: canvasFormat,
  });

  // Create a buffer with the vertices for a single cell
  const vertices = new Float32Array([
    -0.8, -0.8,
     0.8, -0.8,
     0.8,  0.8,
    -0.8, -0.8,
     0.8,  0.8,
    -0.8,  0.8,
  ]);
  const vertexBuffer = device.createBuffer({
    label: "Cell vertices",
    size: vertices.byteLength,
    usage: GPUBufferUsage.VERTEX | GPUBufferUsage.COPY_DST,
  });
  device.queue.writeBuffer(vertexBuffer, 0, vertices);

  const vertexBufferLayout: GPUVertexBufferLayout = {
    arrayStride: 8,
    attributes: [{
      format: "float32x2",
      offset: 0,
      shaderLocation: 0,
    }],
  };

  const bindGroupLayout = device.createBindGroupLayout({
    label: "Cell Bind Group Layout",
    entries: [
      {
        binding: 0,
        visibility: GPUShaderStage.VERTEX | GPUShaderStage.COMPUTE,
        buffer: {},
      },
      {
        binding: 1,
        visibility: GPUShaderStage.VERTEX | GPUShaderStage.COMPUTE,
        buffer: { type: "read-only-storage" },
      },
      {
        binding: 2,
        visibility: GPUShaderStage.COMPUTE,
        buffer: { type: "storage" },
      },
    ],
  });

  const pipelineLayout = device.createPipelineLayout({
    label: "Cell Pipeline Layout",
    bindGroupLayouts: [bindGroupLayout],
  });

  const cellShaderModule = device.createShaderModule({
    label: "Cell Shader",
    code: shaderCode,
  });

  const cellPipeline = device.createRenderPipeline({
    label: "Cell pipeline",
    layout: pipelineLayout,
    vertex: {
      module: cellShaderModule,
      entryPoint: "vertexMain",
      buffers: [vertexBufferLayout],
    },
    fragment: {
      module: cellShaderModule,
      entryPoint: "fragmentMain",
      targets: [{
        format: canvasFormat,
      }],
    },
  });

  const simulationShaderModule = device.createShaderModule({
    label: "Life simulation shader",
    code: computeCode,
  });

  const simulationPipeline = device.createComputePipeline({
    label: "Simulation pipeline",
    layout: pipelineLayout,
    compute: {
      module: simulationShaderModule,
      entryPoint: "computeMain",
    },
  });

  const uniformArray = new Float32Array([GRID_SIZE, GRID_SIZE]);
  const uniformBuffer = device.createBuffer({
    label: "Grid Uniforms",
    size: uniformArray.byteLength,
    usage: GPUBufferUsage.UNIFORM | GPUBufferUsage.COPY_DST,
  });
  device.queue.writeBuffer(uniformBuffer, 0, uniformArray);

  const cellStateArray = new Uint32Array(GRID_SIZE * GRID_SIZE);
  const cellStateStorage = [
    device.createBuffer({
      label: "Cell State A",
      size: cellStateArray.byteLength,
      usage: GPUBufferUsage.STORAGE | GPUBufferUsage.COPY_DST,
    }),
    device.createBuffer({
      label: "Cell State B",
      size: cellStateArray.byteLength,
      usage: GPUBufferUsage.STORAGE | GPUBufferUsage.COPY_DST,
    }),
  ];

  for (let i = 0; i < cellStateArray.length; ++i) {
    cellStateArray[i] = Math.random() > 0.6 ? 1 : 0;
  }
  device.queue.writeBuffer(cellStateStorage[0], 0, cellStateArray);

  const bindGroups = [
    device.createBindGroup({
      label: "BindGroup A",
      layout: bindGroupLayout,
      entries: [
        { binding: 0, resource: { buffer: uniformBuffer }},
        { binding: 1, resource: { buffer: cellStateStorage[0] }},
        { binding: 2, resource: { buffer: cellStateStorage[1] }},
      ],
    }),
    device.createBindGroup({
      label: "BindGroup B",
      layout: bindGroupLayout,
      entries: [
        { binding: 0, resource: { buffer: uniformBuffer }},
        { binding: 1, resource: { buffer: cellStateStorage[1] }},
        { binding: 2, resource: { buffer: cellStateStorage[0] }},
      ],
    }),
  ];

  let step = 0;
  function updateGrid(): void {
    const encoder = device.createCommandEncoder();

    // Compute pass
    const computePass = encoder.beginComputePass();
    computePass.setPipeline(simulationPipeline);
    computePass.setBindGroup(0, bindGroups[step % 2]);
    const workgroupCount = Math.ceil(GRID_SIZE / WORKGROUP_SIZE);
    computePass.dispatchWorkgroups(workgroupCount, workgroupCount);
    computePass.end();

    // Render pass
    const pass = encoder.beginRenderPass({
      colorAttachments: [{
        view: context.getCurrentTexture().createView(),
        loadOp: "clear",
        clearValue: { r: 0, g: 0, b: 0.4, a: 1.0 },
        storeOp: "store",
      }]
    });

    pass.setPipeline(cellPipeline);
    pass.setBindGroup(0, bindGroups[step % 2]);
    pass.setVertexBuffer(0, vertexBuffer);
    pass.draw(vertices.length / 2, GRID_SIZE * GRID_SIZE);
    pass.end();

    device.queue.submit([encoder.finish()]);
    step++;
  }

  setInterval(updateGrid, UPDATE_INTERVAL);
}

init();
