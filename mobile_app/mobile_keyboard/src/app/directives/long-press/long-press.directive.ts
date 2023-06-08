import { AfterViewInit, Directive, ElementRef, Input, Output, EventEmitter } from '@angular/core';
import { GestureController } from '@ionic/angular';

@Directive({
  selector: '[appLongPress]'
})
export class LongPressDirective implements AfterViewInit {

  @Output() press = new EventEmitter()
  @Input('delay') delay = 50;
  
  private active: boolean = false;
  private action: any;


  constructor(private el: ElementRef,
    private gestureCttrl: GestureController) { }
  
  
  ngAfterViewInit(): void {
    this.initGesture()
  
  }

  initGesture() {
    let gesture = this.gestureCttrl.create({
      el: this.el.nativeElement,
      gestureName: 'long-press',
      threshold: 0,
      onStart: (ev) => {
        console.log('gesture started')
        this.active = true
        this.longPressCheck()
      },

      onEnd: (ev) => {
        console.log('gesture ended')
        this.active = false
      }
    }, true);
    gesture.enable();
  }

  longPressCheck() {
    if (this.action) {
      clearTimeout(this.action);
    }
    
    this.action = setTimeout(() => {
      if (this.active) {
        console.log("Long preeeeeess")
        this.press.emit();
      } 
    }, 40);
  }

}
