import {Scene} from "three";
import {Sizes} from "./utils/sizes";
import {Time} from "./utils/time";

export class Visualiser {
  private readonly scene: Scene;
  private readonly sizes: Sizes;
  private readonly time: Time;

  constructor(canvas: HTMLCanvasElement, window: Window) {
    this.sizes = new Sizes(canvas, window);
    this.time = new Time(window);

    this.scene = new Scene();
  }


  destroy() {
      this.sizes.destroy();
      this.time.destroy();
  }

  private update() {

  }

  private resize() {

  }
}
