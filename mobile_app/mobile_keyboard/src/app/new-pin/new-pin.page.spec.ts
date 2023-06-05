import { ComponentFixture, TestBed } from '@angular/core/testing';
import { NewPinPage } from './new-pin.page';

describe('NewPinPage', () => {
  let component: NewPinPage;
  let fixture: ComponentFixture<NewPinPage>;

  beforeEach(async(() => {
    fixture = TestBed.createComponent(NewPinPage);
    component = fixture.componentInstance;
    fixture.detectChanges();
  }));

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
