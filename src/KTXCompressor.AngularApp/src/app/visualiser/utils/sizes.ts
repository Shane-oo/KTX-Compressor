import {Subject} from "rxjs";

export class Sizes {
  resized: Subject<void>;
  width: number;
  height: number;
  pixelRatio: number
  private readonly canvas: HTMLCanvasElement;
  private readonly window: Window;
  private resizeTimeout: any;

  constructor(canvas: HTMLCanvasElement, window: Window) {
    this.canvas = canvas;
    this.window = window;

    this.width = canvas.width;
    this.height = canvas.height;
    this.pixelRatio = Math.min(window.devicePixelRatio, 2);

    this.resized = new Subject();

    this.window.addEventListener('resize', this.resize);
  }

  destroy() {
    this.window.removeEventListener('resize', this.resize);
    this.resized.complete();
  }

  private resize = () => {
    if (this.resizeTimeout) {
      clearTimeout(this.resizeTimeout);
    }

    this.resizeTimeout = setTimeout(() => {
      this.width = this.canvas.clientWidth;
      this.height = this.canvas.clientHeight;
      this.pixelRatio = Math.min(this.window.devicePixelRatio, 2);

      this.resized.next();
    }, 100);
  };

}
