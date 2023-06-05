import { ComponentFixture, TestBed } from '@angular/core/testing';
import { T1Page } from './t1.page';

describe('T1Page', () => {
  let component: T1Page;
  let fixture: ComponentFixture<T1Page>;

  beforeEach(async(() => {
    fixture = TestBed.createComponent(T1Page);
    component = fixture.componentInstance;
    fixture.detectChanges();
  }));

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
