import {Subject} from "rxjs";

export class Time {
  tickSubject: Subject<void>;
  private readonly window: Window;
  private readonly start: number;
  private current: number;
  private elapsed: number;
  private delta: number;
  private destroyed: boolean = false;
  private animationFrameRequestId: number = 0;

  constructor(window: Window) {
    this.window = window;

    this.tickSubject = new Subject();
    this.start = Date.now();
    this.current = this.start;
    this.elapsed = 0;
    this.delta = 16; // 16 fps default

    this.window.requestAnimationFrame(() => {
      this.tick();
    })
  }

  destroy() {
    this.destroyed = true;
    this.tickSubject.complete();
  }

  private tick() {
    if (this.destroyed) {
      this.window.cancelAnimationFrame(this.animationFrameRequestId);
      return;
    }

    const currentTime = Date.now();
    this.delta = currentTime - this.current;
    this.current = currentTime;
    this.elapsed = this.current - this.start;

    this.tickSubject.next();

    this.animationFrameRequestId = window.requestAnimationFrame(() => {
      this.tick();
    });
  }
}
