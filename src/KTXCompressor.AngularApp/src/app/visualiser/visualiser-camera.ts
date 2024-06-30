import {Sizes} from "./utils/sizes";
import {PerspectiveCamera, Scene} from "three";

export class VisualiserCamera {
  private readonly canvas: HTMLCanvasElement;
  private readonly sizes: Sizes;
  private readonly scene: Scene;
  private readonly camera: PerspectiveCamera;

  constructor(canvas: HTMLCanvasElement, sizes: Sizes, scene: Scene) {
    this.canvas = canvas;
    this.sizes = sizes;
    this.scene = scene;

    this.camera = this.createCamera();
    this.scene.add(this.camera);


  }

  update() {

  }

  destroy() {
    this.scene.remove()
  }

  private createCamera(): PerspectiveCamera {
    const fov = 35;
    const aspect = this.sizes.width / this.sizes.height;
    const near = 0.1; // m
    const far = 100; // m
    return new PerspectiveCamera(fov, aspect, near, far);
  }
}
