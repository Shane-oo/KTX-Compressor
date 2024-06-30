import {AfterViewInit, Component, ElementRef, Inject, OnDestroy, ViewChild} from '@angular/core';
import {Visualiser} from "./visualiser";
import {DOCUMENT} from "@angular/common";

@Component({
  selector: 'app-visualiser',
  standalone: true,
  imports: [],
  templateUrl: './visualiser.component.html',
  styleUrl: './visualiser.component.css'
})
export class VisualiserComponent implements OnDestroy, AfterViewInit {
  @ViewChild('canvas', {static: false}) canvas: ElementRef | null = null;
  private visualiser: Visualiser | null = null;


  constructor(@Inject(DOCUMENT) private document: Document) {
  }

  ngOnDestroy(): void {
    this.visualiser?.destroy();
  }

  ngAfterViewInit(): void {
    if (this.canvas?.nativeElement && this.document.defaultView) {
      this.visualiser = new Visualiser(this.canvas?.nativeElement, this.document.defaultView);
    } else {
      console.error("Something is wrong with canvas or window");
    }
  }

}
